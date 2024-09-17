 <%--
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 --%>
<!doctype html>
<%@page contentType="text/html" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<html>
<head>
<title>DeployHub</title>
<meta name="robots" content="noindex, nofollow" />
<meta http-equiv="Cache-Control" content="no-cache, no-store, must-revalidate" />
<meta http-equiv="Pragma" content="no-cache" />
<meta http-equiv="Expires" content="0" />

<link rel="icon" href="favicon.ico">
<link rel='stylesheet' href='https://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,700italic,800italic,600,400,700,800'>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.13.2/themes/cupertino/jquery-ui.min.css" integrity="sha512-9SEz2+WNpf9iyTy855xgweUvhu8Rz9BnU4d/MBFSFSh4kaAFeCA+/Otj5/NaB9PiMpjO1ajRNbplQTXa182X9A==" crossorigin="anonymous" referrerpolicy="no-referrer" />
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.13.2/themes/cupertino/theme.min.css" integrity="sha512-+Ai9V7NStMKCL1dF/876OSoFPtHzP0zEmPsv4pSqflcnghjeleMeOuCcleYQ2WSn1G+VIY3aYDCBKqmrV8yvAA==" crossorigin="anonymous" referrerpolicy="no-referrer" />
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/jquery.jqplot.min.css" integrity="sha512-x1sYGOeCnohzDAuU02Ecjrjcyiy3iYTGGQjuiFenhGpoQsEtK2V+/ELYnOGFjFY7LDx609Ndm6O01CzRBkZsxA==" crossorigin="anonymous" referrerpolicy="no-referrer" />
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/vis/4.21.0/vis.min.css" integrity="sha512-8OVtKyemDe8Koi/74dnSJughhkwnzqq217zUtUumyvx0Z0kF6H0YYFobH2kEYjKMNq2xNQyN64dnleqecTSL0A==" crossorigin="anonymous" referrerpolicy="no-referrer" />
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/jquery-ui-timepicker-addon/1.6.3/jquery-ui-timepicker-addon.min.css" integrity="sha512-LT9fy1J8pE4Cy6ijbg96UkExgOjCqcxAC7xsnv+mLJxSvftGVmmc236jlPTZXPcBRQcVOWoK1IJhb1dAjtb4lQ==" crossorigin="anonymous" referrerpolicy="no-referrer" />
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/jquery-confirm/3.3.4/jquery-confirm.min.css" integrity="sha512-0V10q+b1Iumz67sVDL8LPFZEEavo6H/nBSyghr7mm9JEQkOAm91HNoZQRvQdjennBb/oEuW+8oZHVpIKq+d25g==" crossorigin="anonymous" referrerpolicy="no-referrer" />
<link rel="stylesheet" href="https://cdn.datatables.net/1.10.19/css/jquery.dataTables.min.css" integrity="sha512-LEcCbgUWPBUGbPctNaH+oxZRo415/rpwfqUQXcT4gS+gZyUTb8OIBV288vKAPLqbqsskLKQxgXOz4fr794XZhg==" crossorigin="anonymous">
<link rel="stylesheet" href="https://cdn.datatables.net/buttons/1.5.6/css/buttons.dataTables.min.css" integrity="sha512-CcNusIkGgfUxM0NVvaVf61WMPiopR/PWjXHjKrDG54v8B/KsK+sePEwc7UAH+13BI4/NDtIgiNCheqO56q1hnw==" crossorigin="anonymous">
<link rel="stylesheet" href="https://cdn.datatables.net/responsive/2.2.3/css/responsive.dataTables.min.css" integrity="sha512-UaJaTZ9jEmxC/slDMtUmSWExaVsOAfbDnBqu32JnAf21+l6v7Pr5wPDRMv7bgtTK+n2LoPsacuxdLWQ/bsd+mA==" crossorigin="anonymous">
<link rel="stylesheet" href="https://cdn.datatables.net/select/1.3.0/css/select.dataTables.min.css" integrity="sha512-VYZZbVQ9p1e1QAj0p6N09j9nAB6RZANnRApDMY1EEg0Fc5m4BNhwJHVLzrW9mnkvlTkJu1GZiJ7uoQl2EIrAHQ==" crossorigin="anonymous">
<link rel="stylesheet" href="https://cdn.datatables.net/rowgroup/1.1.3/css/rowGroup.dataTables.min.css" integrity="sha512-mAkv8Z2O8S7IwSg+9DrxF5jlp5D1D/tYSXMpGnwdbw7CC3sDYPTBS2dMGkc/kOb+2cW3zC2wxgcxbhm7oFBjig==" crossorigin="anonymous">
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/jquery-confirm/3.3.2/jquery-confirm.min.css" integrity="sha512-bpSf9l8verc/pxnDZMoMS2dZ57O7PJ0vaIvajQ1g8jxXVtUnzqTuxEVIDJYrPr/wAAwJ0P9+XPBwoYIOmKBrTA==" crossorigin="anonymous">
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/5.2.0/swagger-ui.min.css" integrity="sha512-ACiaHGAf7OAAY0wf7bnPVobNBsqh8oAf9hn5VHFQvbL6f0yfV8jXGEv+PYQZgEQLrrjcM4GSRDNIV3BG1XobfA==" crossorigin="anonymous" referrerpolicy="no-referrer" />
<link rel="stylesheet" type="text/css" href="css/jquery.dmcontextmenu.css" />
<link rel="stylesheet" type="text/css" href="css/calendar.css" />
<link rel="stylesheet" type="text/css" href="css/calendar-print.css" media="print" />
<link rel="stylesheet" type="text/css" href="css/visualsearch-datauri.css" />
<link rel="stylesheet" type="text/css" href="css/devops_frydesign.css" />

<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.7.0/jquery.min.js" integrity="sha512-3gJwYpMe3QewGELv8k/BX9vcqhryRdzRMxVfq6ngyWXwo03GFEzjsUm8Q7RZcHPHksttq7/GFoxjCVUjkjvPdw==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.13.2/jquery-ui.min.js" integrity="sha512-57oZ/vW8ANMjR/KQ6Be9v/+/h6bq9/l3f0Oc7vn6qMqyhvPd1cvKBRWWpzu0QoneImqr2SkmO4MSqU+RpHom3Q==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-cookie/1.4.1/jquery.cookie.min.js" integrity="sha512-3j3VU6WC5rPQB4Ld1jnLV7Kd5xr+cq9avvhwqzbH/taCRNURoeEpoPBK9pDyeukwSxwRPJ8fDgvYXd6SkaZ2TA==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/jquery.jqplot.min.js" integrity="sha512-FQKKXM+/7s6LVHU07eH2zShZHunHqkBCIcDqodXfdV/NNXW165npscG8qOHdxVsOM4mJx38Ep1oMBcNXGB3BCw==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/plugins/jqplot.barRenderer.min.js" integrity="sha512-3bSkTDeICvspsuOVk5OoL165Cw/+7/9l2dBLQfSxdEni9mqsU8bClh1jE6qkoP77m21dfNU3Nh7hpYo3XPNEEw==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/plugins/jqplot.pieRenderer.min.js" integrity="sha512-aTgJc5LAZwVJGYScGUq8unQx1qYLrqtrTBAVFaLQoJ00sFc3rPrgFbeQzO5POMW19QE+qqjF78tuPF7H/FtRTA==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/plugins/jqplot.canvasTextRenderer.min.js" integrity="sha512-MhByCYIQAHIXcGKYOOp8owMra95/Vzf657UXZ4X6znKLvaGdPJ4qx/9C0fMHImVDXAIn5zbHSJEjdUdARNhksQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/plugins/jqplot.canvasAxisLabelRenderer.min.js" integrity="sha512-jyWWi85r3j7GnE61AF6IHvTcwsqNC++XaC8HpzQzUs930S/ndGKo1uMx3YuGWqgtomnp3HdrQc/NN9AbNYK1HQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/plugins/jqplot.canvasAxisTickRenderer.min.js" integrity="sha512-iqILh+eW3DUTi36x3sGv2Cs4FJiU0UBUqo57zE5uZKIdoUMa7XvU5P4zolR5qyTl9B0mpIAKLoBqjW8aU9+z2g==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/plugins/jqplot.canvasOverlay.min.js" integrity="sha512-AyCAP71iNwLJyBSGWNSWAslW1oeNhDEhe2av+hZWgtKAvKEIU6E1qkNectO+W+AvE9VVke4/zlVZwiJlrhtsXQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/plugins/jqplot.categoryAxisRenderer.min.js" integrity="sha512-G30pMFO9qn/AoGm7+TgY5oenrdWSCK4Z/2r94O7ytbOfBJsbbIW+pUfHfaVD/fo/QRLeutbTAW/wFFks0ojVhQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/plugins/jqplot.highlighter.min.js" integrity="sha512-TR38mFALO4a3kHTCbtQIZvn0qtemTauEJx5rnuXQsHaRLAHyHEQdV+KVC81GZ6hgNqGNukESKnsrkQfTj6uUGg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/plugins/jqplot.cursor.min.js" integrity="sha512-PZeegvZIGsxY47yfPXMap/rdwTmcRAFKFXWaMk7clCGbN2qH7aIh981TpZQXfZVv+JjJ2TzTFghGkuMFpwvE7w==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jqPlot/1.0.9/plugins/jqplot.json2.min.js" integrity="sha512-qDiBj83PaDXcDn22Lz3R+vVJ2JbSgCt5e5rWJgel30lblsJDauDya1mf8O+NQ46oegtSDpfUcxu0XTmYmUeWbg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-json/2.6.0/jquery.json.min.js" integrity="sha512-QE2PMnVCunVgNeqNsmX6XX8mhHW+OnEhUhAWxlZT0o6GFBJfGRCfJ/Ut3HGnVKAxt8cArm7sEqhq2QdSF0R7VQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-ui-timepicker-addon/1.6.3/jquery-ui-timepicker-addon.min.js" integrity="sha512-s5u/JBtkPg+Ff2WEr49/cJsod95UgLHbC00N/GglqdQuLnYhALncz8ZHiW/LxDRGduijLKzeYb7Aal9h3codZA==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/vis/4.21.0/vis.min.js" integrity="sha512-XHDcSyqhOoO2ocB7sKOCJEkUjw/pQCJViP1ynpy+EGh/LggzrP6U/V3a++LQTnZT7sCQKeHRyWHfhN2afjXjCg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/oauth-io/0.6.2/oauth.min.js" integrity="sha512-7vvcd3FgS/LUBJakuT2J5LsNx4FbBN0sJvqIpZlMhQZlbCuQrAHPwP99ksbr01eFHwfnHJxxjCDjXVbNlD7FSg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery.form/4.3.0/jquery.form.min.js" integrity="sha512-YUkaLm+KJ5lQXDBdqBqk7EVhJAdxRnVdT2vtCzwPHSweCzyMgYV/tgGF4/dCyqtCC2eCphz0lRQgatGVdfR0ww==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery.fileDownload/1.4.2/jquery.fileDownload.min.js" integrity="sha512-MZrUNR8jvUREbH8PRcouh1ssNRIVHYQ+HMx0HyrZTezmoGwkuWi1XoaRxWizWO8m0n/7FXY2SSAsr2qJXebUcA==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-confirm/3.3.4/jquery-confirm.min.js" integrity="sha512-zP5W8791v1A6FToy+viyoyUUyjCzx+4K8XZCKzW28AnCoepPNIXecxh9mvGuy3Rt78OzEsU+VCvcObwAMvBAww==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/5.2.0/swagger-ui-bundle.min.js" integrity="sha512-m8Q0/9O6zc8UvJfyorpxEF/mpWLRngRfG4mKb/wuH9cPVlJqhEh4NjcyUv4dSOvhtnfYj8Y9B5vWqPjg85kbZA==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/5.2.0/swagger-ui-standalone-preset.min.js" integrity="sha512-x3AkGS1eF0Jm7aCPsIaHZEL64/K0esWlWZtVVEjNdHOudMeBgD5RXu2JoJjsoQY/wiXISPLp2z4M7U/0mGb5lQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/showdown/2.1.0/showdown.min.js" integrity="sha512-LhccdVNGe2QMEfI3x4DVV3ckMRe36TfydKss6mJpdHjNFiV07dFpS2xzeZedptKZrwxfICJpez09iNioiSZ3hA==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/d3/7.8.5/d3.min.js" integrity="sha512-M7nHCiNUOwFt6Us3r8alutZLm9qMt4s9951uo8jqO4UwJ1hziseL6O3ndFyigx6+LREfZqnhHxYjKRJ8ZQ69DQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jszip/3.10.1/jszip.min.js" integrity="sha512-XMVd28F1oH/O71fzwBnV7HucLxVwtxf26XV8P4wPk26EDxuGZ91N8bsOttmnomcCD3CS5ZMRL50H0GgOHvegtg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/pdfmake/0.2.7/pdfmake.min.js" integrity="sha512-a9NgEEK7tsCvABL7KqtUTQjl69z7091EVPpw5KxPlZ93T141ffe1woLtbXTX+r2/8TtTvRX/v4zTL2UlMUPgwg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/pdfmake/0.2.7/vfs_fonts.min.js" integrity="sha512-P0bOMePRS378NwmPDVPU455C/TuxDS+8QwJozdc7PGgN8kLqR4ems0U/3DeJkmiE31749vYWHvBOtR+37qDCZQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/underscore.js/1.13.6/underscore.min.js" integrity="sha512-Gie1sDqmIXWPoOz5rFFWtxIBSNsfe730tZCEauATECxvglm3nbKKZlnDf7/V47LEIe0vkFw0UkbUOm9tqtZS4A==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/backbone.js/1.4.1/backbone.min.js" integrity="sha512-qxgXCtVclzwNBXcgoQDGsSQe00qExwniiHm+HO4qejaOeXj4/2MGJl7XS+TjiHZ000D1mKRW9oUiQauxSnr80A==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/globalize/0.1.1/globalize.min.js" crossorigin="anonymous"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/globalize/0.1.1/cultures/globalize.culture.de-DE.min.js" integrity="sha512-SOY5OqimmQnWQzzFHK6DL0NVogejB/3FT9G00ljZW6SepTTPjecq5zwrOMaJRzWxl48KYMl7ym02UD9DD18Bng==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/globalize/0.1.1/cultures/globalize.culture.en-GB.min.js" integrity="sha512-8ybmpqPRVcqtjOq5cys2bWCEyDGRiBq/J9M3AKoRXR33vN4QNHY1/vEtLyrM3fZ2OLXmWg0099HX/MVAz7b1IA==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/globalize/0.1.1/cultures/globalize.culture.en-US.min.js" integrity="sha512-roQ/+rEc/Ff1mBdALXauOMxhtmGql/UPUZh/WHUXhSssVNQkjJIx4O8r5iAAPj3kS3bJi0yZ0eA1mr4cdmf2Bg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-sessiontimeout@1.1.0/jquery.sessionTimeout.min.js" integrity="sha256-Ix6ZTtVH5vyqTz11X8KSxjCLVTGUh2IDlojbdRCwouM=" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery.ui.widget@1.10.3/jquery.ui.widget.js" integrity="sha512-fnhNgCpE8JcF8xvsRU+wgA2DSOHyr6+ZkmV5PybH71Y+IsvZLnmUXEwI+shkzfRGseG7Z6KcdE421pdRxfqvug==" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-blockui@2.7.0/jquery.blockUI.min.js" crossorigin="anonymous"></script>
<script src="https://cdn.datatables.net/1.10.19/js/jquery.dataTables.min.js" integrity="sha512-c2bQrjZ1iFm8q28SJKvX+A/H6ItQ+jumpms8XGVRkpc2lWxiNl1WcoI4pf0g5/1tDfcShv7oTwzWF3qizR03ZQ==" crossorigin="anonymous"></script>
<script src="https://cdn.datatables.net/buttons/1.5.6/js/dataTables.buttons.min.js" integrity="sha512-uT8p7/mpKoahsdDmAnB3SnJ2K+nra0rZdMbnDleK43bysgLjq1L8tSsP1cBb+DZJSeY2himH/gF4MsWcMZ1DSg==" crossorigin="anonymous"></script>
<script src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.flash.min.js" integrity="sha512-OXWGUAvUDZVkoYTlDDWSeyxp3lMSCBhQtLyUOizHQtusq/krTINZ1OWwzrf+8qQW3Ng0f89UhOdBt1wddwEQKA==" crossorigin="anonymous"></script>
<script src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.html5.min.js" integrity="sha512-i6U13L2Z182HBSt5k9d+Pcm1vpPIqAGymKzEhfCMFjDJLmm6kW7OQqcZdBE168mZbvRs08jjp9ED995fPg1JFw==" crossorigin="anonymous"></script>
<script src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.print.min.js" integrity="sha512-M2WF3C4rL4Nx73w0Re+xGQK7eoj7y59vhqXBbMJXuDpwYrg3QJcRrv8H+8g/UfqsKrkw8mgZ3eIQ8Cymg62oxw==" crossorigin="anonymous"></script>
<script src="https://cdn.datatables.net/responsive/2.2.3/js/dataTables.responsive.min.js" integrity="sha512-naOcVQyv76gYEkC4g/L6b9qFZJimq0gkU4SdxbXbhDDDlXMJn2ipgGzEQRdSWwwq2sh3FO7HA+d6Rh+HfWRXyQ==" crossorigin="anonymous"></script>
<script src="https://cdn.datatables.net/plug-ins/1.10.19/features/pageResize/dataTables.pageResize.min.js" integrity="sha512-lm6XtgbWCr+hUK7GbJnwtxA/5ZFEXpftTCeUkqcwt+Ay6A8eSbi+2MLnUbEhsiN84IW4dAwypqG7GzlYRndpsQ==" crossorigin="anonymous"></script>
<script src="https://cdn.datatables.net/select/1.3.0/js/dataTables.select.min.js" integrity="sha512-Rcp9jIWdjsuXBccmHAsYqphw8cn+JdkLyEL1CcVLW6ezIly3mx7M5TtbLJ80dRIRIHsfhNEvBGb5YOnpU+Y3iQ==" crossorigin="anonymous"></script>
<script src="https://cdn.datatables.net/rowgroup/1.1.3/js/dataTables.rowGroup.min.js" integrity="sha512-HSgcWBJ9EaK1pFsEashSxcXf4k02zYPPOBjshULtM2HmTctGODi6smeN4Y+1a0Ho5Ts5EzoLSjuQyMGAZ/JH1w==" crossorigin="anonymous"></script>
<script src="https://kit.fontawesome.com/ba98665da8.js" crossorigin="anonymous"></script>
<script src="js/jquery.jstree.js"></script>
<script src="js/visualsearch.js"></script>
<script src="js/fullcalendar.min.js"></script>
<script src="js/date.format.js"></script>
<script src="js/jsPlumb-util.js"></script>
<script src="js/jsPlumb-dom-adapter.js"></script>
<script src="js/jsPlumb-drag.js"></script>
<script src="js/jsPlumb.js"></script>
<script src="js/jsPlumb-endpoint.js"></script>
<script src="js/jsPlumb-connection.js"></script>
<script src="js/jsPlumb-anchors.js"></script>
<script src="js/jsPlumb-defaults.js"></script>
<script src="js/jsPlumb-connectors-statemachine.js"></script>
<script src="js/jsPlumb-connectors-flowchart.js"></script>
<script src="js/jsPlumb-renderers-svg.js"></script>
<script src="js/jsPlumb-renderers-canvas.js"></script>
<script src="js/jsPlumb-renderers-vml.js"></script>
<script src="js/jquery.jsPlumb.js"></script>
<script src="js/selectenter.js"></script>
<script src="js/edit_area/edit_area_full.js"></script>

<script src="js/ao/cookies.js"></script>
<script src="js/ao/prototypes.js"></script>
<script src="js/ao/ao_list.js"></script>
<script src="js/ao/createtree.js"></script>
<script src="js/ao/createtree_selection.js"></script>
<script src="js/ao/GlobalVars.js"></script>
<script src="js/ao/Setup.js"></script>
<script src="js/ao/GetSummary.js"></script>
<script src="js/ao/GetAttributes.js"></script>
<script src="js/ao/GetPrerequisities.js"></script>
<script src="js/ao/GetComponents.js"></script>
<script src="js/ao/GetComponentItems.js"></script>
<script src="js/ao/GetVersionsOfApp.js"></script>
<script src="js/ao/GetVersionsOfComp.js"></script>
<script src="js/ao/GetApplicationVersions.js"></script>
<script src="js/ao/newsfeed.js"></script>
<script src="js/ao/GetReports.js"></script>
<script src="js/ao/GetDeployReports.js"></script>
<script src="js/ao/GetServers4Env.js"></script>
<script src="js/ao/GetServers4Comp.js"></script>
<script src="js/ao/GetComponents4Servers.js"></script>
<script src="js/ao/GetApps4Env.js"></script>
<script src="js/ao/GetEnv4Apps.js"></script>
<script src="js/ao/About.js"></script>
<script src="js/ao/Home.js"></script>
<script src="js/ao/GetGroupMembership.js"></script>
<script src="js/ao/GetUserMembership.js"></script>
<script src="js/ao/GetAdminRights.js"></script>
<script src="js/ao/GetArgs.js"></script>
<script src="js/ao/GetWorkflow.js"></script>
<script src="js/ao/GetProperties.js"></script>
<script src="js/ao/GetNewsDetails.js"></script>
<script src="js/ao/GetEmailTo.js"></script>
<script src="js/ao/jquery.dmcontextmenu.js"></script>
<script src="js/ao/TestServer.js"></script>
<script src="js/ao/GetTemplateBody.js"></script>
<script src="js/ao/DomNav.js"></script>
<script src="js/ao/ao_hook.js"></script>
<script src="js/ao/ao_main.js"></script>

</head>
<body>
<div id="container">

 <div id="header">
  <div class="omlogo"></div>
   <div id="positiondevopsmenu">
	<div class="moduletable_menu">
      <ul class="menu_menu">
      </ul>
	</div>
   </div>

   <div id="positiondevopsmenu_right">
    <div class="moduletable_menu">
      <ul class="menu_menu">
        <li id="about_menu" onclick="ShowAbout()"><i class="fal fa-info-circle fa-xl" aria-hidden="true"  style="padding-right:5px"></i></li>
        <li id="user_menu" onclick="LoadUserProfile()" ><i class="fal fa-user fa-xl" aria-hidden="true"  style="padding-right:5px"></i></li>
        <li id="help_menu"><a class="menu_icon" href="https://docs.ortelius.io/guides/userguide/" target="_blank" ><i class="fal fa-question-circle fa-xl" aria-hidden="true"  style="padding-right:5px"></i></a></li>
        <li id="logout_menu"><a class="menu_icon" href="Logout" ><i class="fal fa-sign-out fa-xl" aria-hidden="true"  style="padding-right:5px"></i></a></li>
      </ul>
	 </div>
    </div>

	<div class="clear"></div>
	</div> <!-- end header -->
	<div id="panel_container_menu" class="left" style="display:none;">
	 <div id="verttabs" class="verttab">
	   <button id="verttab_application" class="tablinks verttab_app" onclick="openList(event, 'application')">Applications</button>
       <button id="verttab_component" class="tablinks verttab_comp" onclick="openList(event, 'component')">Components</button>
       <button id="verttab_domain" class="tablinks verttab_domain" onclick="LoadDomNav()">Domains</button>
       <button id="verttab_environment" class="tablinks verttab_env" onclick="openList(event, 'environment')">Environments</button>
       <button id="verttab_endpoint" class="tablinks verttab_endpoint" onclick="openList(event, 'endpoint')">Endpoints</button>
       <button id="verttab_user" class="tablinks verttab_user" onclick="openList(event, 'user')">Users</button>
       <button id="verttab_group" class="tablinks verttab_group" onclick="openList(event, 'group')">Groups</button>
       <button id="verttab_setup" class="tablinks verttab_setup" onclick="toggleSetup(event, 'setup')">Advanced Features<i class="fad fa-chevron-double-down fa-lg" aria-hidden="true" style="padding-left:10px"></i></button>
       <button id="verttab_action" class="tablinks verttab_action" onclick="openList(event, 'action')">Actions</button>
       <button id="verttab_procedure" class="tablinks verttab_procedure" onclick="openList(event, 'procedure')">Func/Procs</button>
       <button id="verttab_servercomptype" class="tablinks verttab_servercomptype" onclick="openList(event, 'servercomptype')">Customize Types</button>
       <button id="verttab_credential" class="tablinks verttab_credential" onclick="openList(event, 'credential')">Credentials</button>
       <button id="verttab_repository" class="tablinks verttab_repository" onclick="openList(event, 'repository')">Repositories</button>
       <button id="verttab_datasource" class="tablinks verttab_datasource" onclick="openList(event, 'datasource')">Data Sources</button>
       <button id="verttab_notifier" class="tablinks verttab_notifier" onclick="openList(event, 'notifier')">Notifiers</button>
       <button id="verttab_template" class="tablinks verttab_template" onclick="openList(event, 'template')">Notifier Templates</button>
     </div>
	</div>
	<div id="panel_container_right_list" class="right" style="display:none;">
     <div id="applist_pane">
       <div class="list_bar">
          <div id="applist_pane_title" style="display:inline-block;">Latest Versions</div>
          <div id="applist_buttons" style="display:inline-block;">
         	 <button class="title_buttons" onclick="openList(event, 'application')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'application')" style="display:none;"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Base</button>
             <button class="title_buttons" onclick="addRow(event, 'appversion')" style="display:none;"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Version</button>
             <button class="title_buttons" onclick="delRow(event, 'application')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
             <div class="dropdown_menu">
               <button class="title_buttons taskMenuButton" onclick="taskMenu()"><i class="fal fa-tasks" aria-hidden="true" style="padding-right:5px;"></i>Tasks<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div id="apptask_buttons" class="dropdown_menu-content">
               </div>
             </div>
          	 <button class="title_buttons" onclick="openPkgSearch()"><i class="fa-light fa-magnifying-glass" aria-hidden="true" style="padding-right:5px;"></i>Package Search</button>
             <div class="dropdown_menu">
               <button class="title_buttons taskMenuButton" onclick="rptMenu()"><i class="fal fa-file-lines" aria-hidden="true" style="padding-right:5px;"></i>Reports<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div id="rptlist_buttons" class="dropdown_menu-content">
               </div>
             </div>
          </div>
       </div>
          <div id="applist_filter_area" style="height:25px;">
                <div id="app_search_box_container"></div>
          </div>
        <div id="applist_list">
		  <table id="applist" class="stripe hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="applist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Version</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">Environment</th>
				<th style="text-align: left;">Deployment Log</th>
				<th style="text-align: left;">Completed</th>
				<th style="text-align: left;">Result</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">appid</th>
            </tr>
           </thead>
          </table>
        </div>
        <div id="applist_map" height="100%" width="100%"></div>
      </div>
     <div id="complist_pane">
      <div class="list_bar">
        <div id="complist_pane_title" style="display:inline-block;">Latest Versions</div>
          <div id="complist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'component')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <div class="dropdown_menu">
               <button class="title_buttons addMenuButton" onclick="compMenu()" style="color: white;display:none;"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Base<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'docker');">Container</a>
                   <a onclick="addRow(event,'file');">Application File</a>
                   <a onclick="addRow(event,'database');">Database</a>
               </div>
             </div>
             <button class="title_buttons" onclick="addRow(event, 'compversion')" style="display:none;"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Version</button>
             <button class="title_buttons" onclick="delRow(event, 'component')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
          	 <button class="title_buttons" onclick="openDiffDlg(event, objtype, objid)"><i class="fal fa-equals" aria-hidden="true" style="padding-right:5px;"></i>Compare</button>
          	 <button class="title_buttons" onclick="exportCompSBOM(event, objtype, objid)"><i class="fal fa-equals" aria-hidden="true" style="padding-right:5px;"></i>Export SBOM</button>
          </div>
         </div>
          <div id="complist_filter_area" style="height:25px;">
             <div id="comp_search_box_container"></div>
          </div>
         <div id="complist_list">
		  <table class="stripe" id="complist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="complist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Version</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">Environment</th>
				<th style="text-align: left;">Last Deployment Log</th>
				<th style="text-align: left;">Completed</th>
				<th style="text-align: left;">Result</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">compid</th>
            </tr>
           </thead>
          </table>
         </div>
        <div id="complist_map" height="100%" width="100%"></div>
      </div>
      <div id="envlist_pane">
       <div class="list_bar">
        <div id="envlist_pane_title" style="display:inline-block;">Environments</div>
          <div id="envlist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'environmet')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'environment')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'environment')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
             <div class="dropdown_menu">
               <button class="title_buttons envRptMenuButton" onclick="envRptMenu()" style="color: white;"><i class="fal fa-file-contract" aria-hidden="true" style="padding-right:5px;"></i>Reports<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a href="/dmadminweb/reports/EnvSuccessFail.html" target="_blank">Success/Failed Deployments per Environment Report</a>
               </div>
               <div id="envsbomrpt" class="dropdown_menu-content">

               </div>
             </div>
           </div>
        </div>
           <div id="envlist_filter_area" style="height:25px;">
                  <div id="env_search_box_container"></div>
          </div>
         <div id="envlist_list">
		  <table class="stripe" id="envlist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="envlist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Environment</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">envid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="endpointlist_pane">
       <div class="list_bar">
        <div id="endpointlist_pane_title" style="display:inline-block;">Endpoints</div>
          <div id="endpointlist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'endpoint')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'endpoint')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'endpoint')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
       </div>
           <div id="endpointlist_filter_area" style="height:25px;">
                  <div id="endpoint_search_box_container"></div>
          </div>
         <div id="endpointlist_list">
		  <table class="stripe" id="endpointlist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="endpointlist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Endpoint</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">endpointid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="actionlist_pane">
       <div class="list_bar">
        <div id="actionlist_pane_title" style="display:inline-block;">Actions</div>
          <div id="actionlist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'action')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'action')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'action')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
       </div>
           <div id="actionlist_filter_area" style="height:25px;">
                  <div id="action_search_box_container"></div>
          </div>
         <div id="actionlist_list">
		  <table class="stripe" id="actionlist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="actionlist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Action</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">actionid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="procedurelist_pane">
       <div class="list_bar">
        <div id="procedurelist_pane_title" style="display:inline-block;">Func/Procs</div>
          <div id="procedurelist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'procedure')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <div class="dropdown_menu">
               <button class="title_buttons addProcButton" onclick="procMenu()" style="color: white;"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Procedure<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'procedure-ak2');">DMScript in database</a>
                   <a onclick="addRow(event,'procedure-ak3');">Local Script</a>
                   <a onclick="addRow(event,'procedure-ak4');">Endpoint Script</a>
               </div>
              </div>
             <div class="dropdown_menu">
               <button class="title_buttons addFuncButton" onclick="funcMenu()" style="color: white;"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Function<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'function-ak2');">DMScript in database</a>
                   <a onclick="addRow(event,'function-ak3');">Local Script</a>
                   <a onclick="addRow(event,'function-ak4');">Endpoint Script</a>
               </div>
              </div>
             <button class="title_buttons" onclick="delRow(event, 'procedure')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
             <button class="title_buttons" onclick="exportFuncProc()"><i class="fal fa-download" aria-hidden="true" style="padding-right:5px;"></i>Export</button>
             <button class="title_buttons" onclick="importProc()"><i class="fal fa-upload" aria-hidden="true" style="padding-right:5px;"></i>Import</button>
           </div>
        </div>
           <div id="procedurelist_filter_area" style="height:25px;">
                  <div id="procedure_search_box_container"></div>
          </div>
         <div id="procedurelist_list">
		  <table class="stripe" id="procedurelist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="procedurelist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Name</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">Type</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">procedureid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="notifierlist_pane">
       <div class="list_bar">
        <div id="notifierlist_pane_title" style="display:inline-block;">Notifiers</div>
          <div id="notifierlist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'notifier')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
          	 <div class="dropdown_menu">
               <button class="title_buttons addMenuButton" onclick="notifierMenu()" style="color: white;"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'hipchat');">HipChat</a>
                   <a onclick="addRow(event,'slack');">Slack</a>
                   <a onclick="addRow(event,'smtpemail');">Email</a>
                   <a onclick="addRow(event,'txtlocal');">SMS</a>
               </div>
             </div>
             <button class="title_buttons" onclick="delRow(event, 'notifier')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
         </div>
           <div id="notifierlist_filter_area" style="height:25px;">
                  <div id="notifier_search_box_container"></div>
          </div>
         <div id="notifierlist_list">
		  <table class="stripe" id="notifierlist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="notifierlist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Notifier</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">notifierid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="repositorylist_pane">
       <div class="list_bar">
        <div id="repositorylist_pane_title" style="display:inline-block;">Repositories</div>
          <div id="repositorylist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'repository')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
          	 <div class="dropdown_menu">
               <button class="title_buttons addMenuButton" onclick="repoMenu()" style="color: white;"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'filesystem');">File System</a>
                   <a onclick="addRow(event,'http');">Http</a>
                   <a onclick="addRow(event,'meister');">Meister</a>
                   <a onclick="addRow(event,'svn');">SVN</a>
               </div>
              </div>
             <button class="title_buttons" onclick="delRow(event, 'repository')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
        </div>
           <div id="repositorylist_filter_area" style="height:25px;">
                  <div id="repository_search_box_container"></div>
          </div>
         <div id="repositorylist_list">
		  <table class="stripe" id="repositorylist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="repositorylist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Repository</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">repositoryid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="datasourcelist_pane">
       <div class="list_bar">
        <div id="datasourcelist_pane_title" style="display:inline-block;">Data Sources</div>
          <div id="datasourcelist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'datasource')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
          	 <div class="dropdown_menu">
               <button class="title_buttons addMenuButton" onclick="datasourceMenu()" style="color: white;"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'github');">GitHub</a>
                   <a onclick="addRow(event,'jira');">Jira</a>
                   <a onclick="addRow(event,'bugzilla');">Bugzilla</a>
                   <a onclick="addRow(event,'ldap');">LDAP</a>
                   <a onclick="addRow(event,'odbc');">ODBC</a>
               </div>
              </div>
             <button class="title_buttons" onclick="delRow(event, 'datasource')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
        </div>
           <div id="datasourcelist_filter_area" style="height:25px;">
                  <div id="datasource_search_box_container"></div>
          </div>
         <div id="datasourcelist_list">
		  <table class="stripe" id="datasourcelist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="datasourcelist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Data Source</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">datasourceid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="credentiallist_pane">
       <div class="list_bar">
        <div id="credentiallist_pane_title" style="display:inline-block;">Credentials</div>
          <div id="credentiallist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'credential')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
          	 <div class="dropdown_menu">
               <button class="title_buttons addMenuButton" onclick="credMenu()" style="color: white;"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'ck2');">Encrypted in Database</a>
                   <a onclick="addRow(event,'ck5');">Private Key</a>
               </div>
             </div>
             <button class="title_buttons" onclick="delRow(event, 'credential')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
          </div>
           <div id="credentiallist_filter_area" style="height:25px;">
                  <div id="credential_search_box_container"></div>
          </div>
         <div id="credentiallist_list">
		  <table class="stripe" id="credentiallist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="credentiallist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Credential</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">credentialid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="userlist_pane">
       <div class="list_bar">
        <div id="userlist_pane_title" style="display:inline-block;">Users</div>
          <div id="userlist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'user')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'user')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'user')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
         </div>
           <div id="userlist_filter_area" style="height:25px;">
                  <div id="user_search_box_container"></div>
          </div>
         <div id="userlist_list">
		  <table class="stripe" id="userlist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="userlist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">User</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">userid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="grouplist_pane">
       <div class="list_bar">
        <div id="grouplist_pane_title" style="display:inline-block;">Groups</div>
          <div id="grouplist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'group')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
           </div>
       </div>
           <div id="grouplist_filter_area" style="height:25px;">
                  <div id="group_search_box_container"></div>
          </div>
         <div id="grouplist_list">
		  <table class="stripe" id="grouplist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="grouplist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Group</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">groupid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="servercomptypelist_pane">
       <div class="list_bar">
        <div id="servercomptypelist_pane_title" style="display:inline-block;">Types</div>
          <div id="servercomptypelist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'servercomptype')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'servercomptype')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'servercomptype')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
        </div>
           <div id="servercomptypelist_filter_area" style="height:25px;">
                  <div id="servercomptype_search_box_container"></div>
          </div>
         <div id="servercomptypelist_list">
		  <table class="stripe" id="servercomptypelist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="servercomptypelist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Type</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">servercomptypeid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="templatelist_pane">
       <div class="list_bar">
        <div id="templatelist_pane_title" style="display:inline-block;">Notifiers Templates</div>
          <div id="templatelist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'template')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'template')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'template')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
       </div>
           <div id="templatelist_filter_area" style="height:25px;">
                  <div id="template_search_box_container"></div>
          </div>
         <div id="templatelist_list">
		  <table class="stripe" id="templatelist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="templatelist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Template</th>
				<th style="text-align: left;">Notifier</th>
				<th style="text-align: left;">templateid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
     </div>
	<div id="dashboard_panel">
	<div id="panel_container_right" class="right" style="display:none" >
	 <div id="right_panel_header">
       <div id="header_icon"></div>
	 </div>
	 <div id="right_panel_tabs">
	  <div class="tabrow_tabmenu">
	   <div class="moduletable_tabmenu">
	    <ul>
	      <li id="tabs-General" onclick="ActivateSubTabs()" style="display:none">General</li>
          <li id="tabs-PackageComponents" onclick="ActivateSubTabs()" style="display:none">Package Components</li>
          <li id="tabs-DeliveryPipeline" onclick="ActivateSubTabs()" style="display:none">Delivery Pipeline</li>
          <li id="bookmark-Security-Posture" onclick="Scroll2Bookmark('security-posture')" style="display:none"><u>Security Posture</u></li>
          <li id="bookmark-Impact-Assessment" onclick="Scroll2Bookmark('impact-assessment')" style="display:none"><u>Impact Assessment</u></li>
          <li id="bookmark-Proc-Definition" onclick="Scroll2Bookmark('proc-definition')" style="display:none"><u>Function/Procedure Definition</u></li>
          <li id="bookmark-DevOps-Details" onclick="Scroll2Bookmark('devops-details')" style="display:none"><u>Component with DevOps Details</u></li>
          <li id="bookmark-Calendar" onclick="Scroll2Bookmark('calendar-section')" style="display:none"><u>Calendar</u></li>
          <li id="bookmark-Audit-Access" onclick="Scroll2Bookmark('audit-access')" style="display:none"><u>Audit & Access</u></li>
        </ul>
	   </div>
	  </div>
	 </div>
	 <div id="right_panel_main">
	  <div id="right_panel_title_area" class="right_panel_title_area" style="display:none"></div>
	  <div id="title_icon" class="title_icon"></div>
        <div id="tabs-General-data" class="div_scroll" style="display:none">
           <div id="security-posture">Security Posture</div>
           <div id="tabs-General-row-12b">
           	<div id="license-panel">
           	  <div id="license_title" style="text-align:left"">
                     <h2>Software Bill of Materials (SBOM)</h2>
                 </div>
                 <div id="license-data">
                    <div id="licenselist_list" style="width:96%;height:100px;display:inline"></div>
                       <table class="stripe" id="licenselist" width="100%">
                           <thead>
                               <tr>
                                   <th>Package</th>
                                   <th>Version</th>
                                   <th>License</th>
                                   <th>Component</th>
                                   <th>Url</th>
                                   <th style="word-wrap: break-word">OSSF Scorecard</th>
                               </tr>
                           </thead>
                           <tbody></tbody>
                       </table>
                 </div>
             </div>
           </div>
           <div id="tabs-General-row-12a">
           	<div id="cve-panel">
           	  <div id="cve_title" style="text-align:left"">
                     <h2>Vulnerabilities</h2>
                 </div>
                 <div id="cve-data">
                    <div id="cvelist_list" style="width:96%;height:100px;display:inline"></div>
                       <table class="stripe" id="cvelist" width="100%">
                           <thead>
                               <tr>
                                   <th style="padding:0;">&nbsp;</th>
                                   <th>Package</th>
                                   <th>Version</th>
                                   <th>ID</th>
                                   <th>Url</th>
                                   <th>Summary</th>
                                   <th>Component</th>
                               </tr>
                           </thead>
                           <tbody></tbody>
                       </table>
                 </div>
                </div>
             </div>
            <div id="tabs-General-row-scorecard">
           		<div id="scorecard-panel">
           	  		<div id="scorecard_title" style="text-align:left"">
                     	<h2>OpenSSF Scorecard</h2>
                 	</div>
                 <div id="scorecard-data">
                    <div id="scorecardlist_list" style="width:90%;height:150px;display:inline"></div>
                       <table class="stripe" id="scorecardlist" width="100%">
                           <thead>
                               <tr>
                                <th>Risk Level</th>
                             	<th>Score</th>
                                <th style="text-align:left">Check</th>
                               </tr>
                           </thead>
                           <tbody></tbody>
                       </table>
                 </div>
             	</div>
             </div>
             <div id="tabs-General-row-12-0">
               <div id="readme-panel">
            	  <div id="readme_header">
	            	  <div id="readme_title" style="text-align:left"">
	                      <h2>Readme</h2>
	                  </div>
	                  <div id="readme_header_buttons">
                       	<button class="add_button" id="upload_readme"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Upload</button>
	                  </div>
                  </div>
                  <div id="readme-data">
                  </div>
            	</div>
             </div>
	          <div id="tabs-General-row-lic">
	            <div id="licensemd-panel">
          		   <div id="licensemd_header">
	            	  <div id="licensemd_title" style="text-align:left"">
	                     <h2>License</h2>
	                  </div>
	                  <div id="licensemd_header_buttons">
                       	<button class="add_button" id="upload_licensemd"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Upload</button>
	                  </div>
                  </div>
                  <div id="licensemd-data">
                  </div>
            	</div>
	          </div>
            </div>
            <div id="impact-assessment">Impact Assessment</div>
                        <div id="tabs-General-row-20">
                <div id="tabs-General-right-20">
                    <div id="trends-panel">
                        <div id="trends_title" style="text-align:left"">
                            <h2>Trends</h2>
                        </div>
                        <div id="reports-data" style="display:inline-block">
                            <div id="plot1resizable" style="float:left;height:200px;width:400px">
                                <div id="timetodeploylinechart" style="height:96%;width:96%;"></div>
                            </div>
                            <div id="plot2resizable" style="float:left;height:200px;width:200px">
                                <div id="applicationsuccesspiechart" style="height:96%;width:96%;"></div>
                            </div>
                        </div>
                    </div>
                </div>
                <div id="steps-panel">
                        <div id="steps_title" style="text-align:left"">
                            <h2>Current Step Duration</h2>
                        </div>
                        <div id="steps-data" style="display:inline-block">
     						<div id="plot4resizable" style="float:left;height:200px;width:200px">
     						<div id="deploymenttimebystep4" style="height:96%;width:96%;"></div>
     					</div>
                        </div>
                </div>
                <div id="comp4srv-panel">
                  <div id="comp4srv-panel-header">
                    <div id="comp4srv_title">
                        <h2>Deployed Components</h2>
                    </div>
                  </div>
                    <div id="comp4srv-data">
                        <table id="comp4srv" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Current Version</th>
                                    <th>Deployment</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="comp4srv-edit" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Current Version</th>
                                    <th>Deployment</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
                <div id="env4app-panel">
                  <div id="env4app-panel-header">
                    <div id="envs_title">
                        <h2>Assigned Environments</h2>
                    </div>
                    <div id="env4app_header_buttons">
                        	<button class="add_button" onClick="javascript:AddEnv4AppRow('env4app')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditEnv4AppRow('env4app')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteEnv4AppRow('env4app')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveEnv4AppRow('env4app')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelEnv4AppRow('env4app')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    </div>
                  </div>
                    <div>
                        <table id="env4app" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Environment</th>
                                    <th>Details</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="env4app-edit" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Environment</th>
                                    <th>Details</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                 </div>

                 <div id="deployedenv4app-panel">
                  <div id="deployedenv4app-panel-header">
                    <div id="deployed_envs_title">
                        <h2>Deployed Environments</h2>
                    </div>
                  </div>
                    <div id="deployedenv4app-data">
                        <table id="deployedenv4app" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>Environment</th>
                                    <th>Deployment Log</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                 </div>

                 <div id="app4env-panel">
                  <div id="app4env-panel-header">
                    <div id="app4env_title">
                        <h2>Assigned Applications</h2>
                    </div>
                  </div>
                    <div>
                        <table id="app4env" class="dev_table">
                            <thead>
                                <tr>
                                    <th>Application</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                 </div>
                <div id="defect-panel-20">
                        <div id="defects_title">
                            <h2>Change Requests</h2>
                        </div>
                         <div id="defect_header_buttons_20">
                        </div>
                        <div id="defect-20-data">
                        <table id="defect-20" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>CR</th>
                                    <th>Summary</th>
                                    <th>Component</th>
                                    <th>Status</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                 </div>
            </div>
            <div id="tabs-General-row-30">
          		<div id="apps4comp-panel">
            	  <div id="apps4comp_title" style="text-align:left"">
                      <h2>Consuming Applications</h2>
                  </div>
                  <div id="apps4comp-data">
                     <div id="apps4complist_list" style="width:96%;height:100px;display:inline"></div>
                       <table class="stripe" id="apps4complist" width="100%">
                           <thead>
                               <tr>
                                   <th style="text-align:left">Application</th>
                                   <th style="text-align:left">Domain</th>
                                   <th style="text-align:left">Environment</th>
                                   <th style="text-align:left">Deployment Log</th>
                                   <th style="text-align:left">Completed</th>
                                   <th style="text-align:left">Result</th>
                                   <th style="text-align:left">Id</th>
                               </tr>
                           </thead>
                           <tbody></tbody>
                       </table>
                  </div>
                </div>
          		<div id="planning-panel">
                        <div id="planning_title" style="text-align:left"">
                            <h2>Applications scheduled for this Release</h2>
                        </div>
          		        <div id="planningtimeline">
          		        </div>
               </div>
            </div>
            <div id="tabs-General-row-9">
               <div id="appcomp-panel">
                 <div id="appcomp_title" style="text-align:left"">
                     <h2>Component Dependencies</h2>
                 </div>
                 <div id="appcomp-data">
                     <div id="appcomplist_list" style="width:96%;height:100px;display:inline"></div>
                        <table class="stripe" id="appcomplist" width="100%">
                            <thead>
                                <tr>
                                    <th style="text-align:left">Component</th>
                                    <th style="text-align:left">Domain</th>
                                    <th style="text-align:left">Id</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                 </div>
               </div>
             </div>
            <div id="tabs-General-row-40">
	                <div id="defect-panel" style="display:none">
	                    <div id="defects_title">
	                        <h2>Change Requests</h2>
	                    </div>
	                     <div id="defect_header_buttons">
	                    	<button class="add_button" onClick="javascript:AddDefectRow('defect')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
	                    	<button class="edit_button" onClick="javascript:EditDefectRow('defect')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
	                    	<button class="delete_button" onClick="javascript:DeleteDefectRow('defect')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
	                    	<button class="save_button" onClick="javascript:SaveDefectRow('defect')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
	                    	<button class="cancel_button" onClick="javascript:CancelDefectRow('defect')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
	                    </div>
	                    <div id="defect-data">
	                    <table id="defect" class="cb_dev_table">
	                        <thead>
	                            <tr>
	                                <th>&nbsp;</th>
	                                <th>CR</th>
	                                <th>Summary</th>
	                                <th>Component</th>
	                                <th>Status</th>
	                            </tr>
	                        </thead>
	                        <tbody></tbody>
	                    </table>
	                    <table id="defect-edit" class="cb_dev_table">
	                        <thead>
	                            <tr>
	                                <th>&nbsp;</th>
	                                <th>CR</th>
	                                <th>Summary</th>
	                                <th>Component</th>
	                                <th>Status</th>
	                            </tr>
	                        </thead>
	                        <tbody></tbody>
	                    </table>
	                </div>
	             </div>
	             <div id="tabs-General-right">
	                <div id="tabs-General-right-header">
	                    <h2>Blast Radius</h2>
	                </div>
	                <div id="tabs-General-map"></div>
	            </div>
            </div>
            <div id="tabs-General-row-12">
	            	<div id="swagger-panel">
	            	  <div id="swagger_header">
		            	  <div id="swagger_title" style="text-align:left"">
		                            <h2>Swagger</h2>
		                  </div>
		                  <div id="swagger_header_buttons">
	                       	<button class="add_button" id="upload_swagger"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Upload</button>
		                  </div>
	                  </div>
	                  <div id="swagger-data">
	                  </div>
	            	</div>
		    </div>
            <div id="tabs-General-row-15a"  >
                <div id="outerworkflow" class="versionsandinformation">
     	 			<div class="versions">
	     				<div id="innerworkflow" style="height:600px;width:100%;">
		 				</div>
	    			</div>
       			</div>
    			<div id="pickproc" class="accordionpanel">
	     			<h4>Function/Procedure</h4>
   	     			 <div id="fragmentlist" align="left"></div>
        		</div>
               <div id="srv4env-panel">
                 	<div id="srv4env-header">
                     	<div id="srv4env_title">
                      	  <h2>Assigned Endpoints</h2>
                    	</div>
                    	<div id="srv4env_header_buttons">
                        	<button class="add_button" onClick="javascript:AddSrv4EnvRow('srv4env')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditSrv4EnvRow('srv4env')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteSrv4EnvRow('srv4env')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveSrv4EnvRow('srv4env')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelSrv4EnvRow('srv4env')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    	</div>
                    </div>
                    <div id="srv4env-data">
                        <table id="srv4env" class="cb_dev_table" width="100%" >
                            <thead>
                                <tr>
                                    <th></th>
                                    <th>Name</th>
                                    <th>Hostname</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="srv4env-edit" class="cb_dev_table" width="100%" >
                            <thead>
                                <tr>
                                    <th></th>
                                    <th>Name</th>
                                    <th>Hostname</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
              <div id="emailhelp-panel" style="display:none">
                <div id="emailhelp_title">
                      	  <h2>Using Variables</h2>
                </div>
                <div id="emailhelp_data">
          When constructing the subject line and message body for a template, you can use
          <I>Variables</I>. These variables are automatically expanded when the notification
          is issued.<br><br>
          Variables can be either <I>Object Methods</I> (such as \${application.name}) or
          <I>Scalar Variables</I> that are set by DeployHub during specific operations
          (such as $SERVER_STATUS during a periodic server scan).<br><br>Object Methods can be used for
          any object that is available to DeployHub during the operation. For example, you could
          use \${application.id} to get the internal application id or \${application.domain.name} to get
          the current domain for the application.<br><br>
          The table below shows which variables are set during specific operations.<br><br>
          <table id="emailhelptab" class="dev_table" style="white-space:nowrap">
          <thead>
          <tr><th>Variable Name</th><th>Description</th><th>Operation</th></tr>
          </thead>
          <tbody>
          <tr ><td>\${application.name}</td><td>The name of the current application</td><td>Any DeployHub task operating on an Application (such as <i>Deploy, Move, Approve</i> etc)</td></tr>
 	 	  <tr><td>\${environment.name}</td><td>The name of the target environment</td><td>Any DeployHub task operating on an Environment (such as <i>Deploy</i> etc)</td></tr>
		  <tr ><td>\${user.name}</td><td>The name of the invoking user</td><td>Any DeployHub task.</td></tr>
		  <tr><td>\$DEPLOY_ID</td><td>The Deployment ID Number</td><td>Set during a Deploy task.</td></tr>
          <tr ><td>\$SERVER_NAME</td><td>Endpoint Name</td><td>The name of the end point during an automatic periodic scan (AutoPing or AutoMD5). Use this variable in a Ping or MD5 template.</td></tr>
          <tr><td>\$SERVER_HOSTNAME</td><td>Endpoint hostname</td><td>The hostname of the end point during an automatic periodic scan (AutoPing or AutoMD5). Use this variable in a Ping or MD5 template.</td></tr>
          <tr ><td>\$SERVER_STATUS</td><td>OK or FAILED</td><td>The status of the server during an automatic periodic scan (AutoPing). Use this variable in a Ping template.
          Note this variable is always OK in an MD5 template since MD5 scans are only performed if the ping check succeeds.</td></tr>
          <tr><td>\$NAME_RESOLUTION</td><td>OK or FAILED</td><td>Set during an automatic periodic scan (AutoPing). Reflects the success or failure of resolution of hostname. Use this variable in a Ping template.</td></tr>
          <tr ><td>\$PING</td><td>OK or FAILED</td><td>Set during an automatic periodic scan (AutoPing). Reflects the success or failure of the ping to the server. Use this variable in a Ping template.</td></tr>
          <tr><td>\$CONNECTION</td><td>OK or FAILED</td><td>Set during an automatic periodic scan (AutoPing). Reflects the success or failure of the connection attempt to the server using the associated credentials. Use this variable in a Ping template.</td></tr>
          <tr ><td>\$BASEDIR</td><td>OK or FAILED</td><td>Set during an automatic periodic scan (AutoPing). Reflects the success or failure of the attempt to switch to the server's base directory. Use this variable in a Ping template.</td></tr>
          <tr><td>\$MD5_FILELIST</td><td>A list of files (one per line)</td><td>Set during an automatic periodic scan (AutoMD5). Lists all the files whose MD5 checksum differs from that calculated during the last deployment to the server. Use this variable in a MD5 template.</td></tr>
          <tr ><td>\$SRCDOMAIN</td><td>The name of the Source Domain</td><td>Set during a <I>Move</I> or <I>Approve</I> task. The name of the Domain the application is being moved <I>from</I> (for a move) or its current domain (for an approve).</td></tr>
          <tr><td>\$TGTDOMAIN</td><td>The name of the Target Domain</td><td>Set during a <I>Move</I> or <I>Approve</I> task. The name of the Domain the application is being moved <I>to</I> or approved <I>for</I>.</td></tr>
          <tr ><td>\$NOTES</td><td>Free Text</td><td>Set during a <I>Move, Approve</I> or <I>Request</I> task. The notes that were entered (if any) by the user when the task was run.</td></tr>
          <tr><td>\$TASKNAME</td><td>The name of the Requested Task</td><td>Set during a <I>Request</I> task. The name of the task being requested to be run.</td></tr>
          <tr ><td>\$ERROR_TEXT</td><td>Last Endpoint Error</td><td>Set during an automatic periodic scan (AutoPing). The last error detected during the connection attempt.</td></tr>
          </tbody>
          </table>
               </div>
              </div>
            </div>
            <div id="tabs-General-row-25">
            </div>
            <div id="tabs-General-row-28">
                 <div id="feedback-panel">
                    <div id="feedback-panel-header">
                        <div id="feedback_title" style="text-align:left"">
                            <h2>Last Deployment Difference based on Environment:</h2>
                        </div>
                    </div>
                    <div id="feedback-data">
                    </div>
                </div>
            </div>
            <div id="tabs-General-row-32">
                <div id="provides-panel" style="display:none">
                   <div id="provides_title" style="text-align:left"">
                        <h2>Providing API End Points</h2>
                    </div>
                     <div id="provides_list">
					  <table id="provideslist" class="hover row-border nowrap" height="100%" width="100%">
			           <thead>
			            <tr>
							<th style="text-align: left;">Verb</th>
							<th style="text-align: left;">API End Point</th>
			            </tr>
			          </thead>
			         </table>
			        </div>
          		</div>
          		<div id="consumes-panel" style="display:none">
                        <div id="consumes_title" style="text-align:left"">
                            <h2>Consuming API End Points</h2>
                        </div>
                     <div id="consumes_list">
					  <table id="consumeslist" class="hover row-border nowrap" height="100%" width="100%">
			           <thead>
			            <tr>
							<th style="text-align: left;">Verb</th>
							<th style="text-align: left;">API End Point</th>
							<th style="text-align: left;">Component</th>
			            </tr>
			          </thead>
			         </table>
			        </div>
               </div>
             </div>
            <div id="proc-definition">Function/Procedure Definition</div>
            <div id="tabs-General-row-9a">
               <div id="inputparams-panel">
                  <div id="inputparams-panel-header">
                    <div id="inputparams_title">
                    	<h2>Input Parameters</h2>
                    </div>
                    <div id="inputparams_header_buttons">
                        	<button class="add_button" onClick="javascript:AddInputParamsRow('inputparams')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditInputParamsRow('inputparams')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteInputParamsRow('inputparams')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveInputParamsRow('inputparams')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelInputParamsRow('inputparams')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                     </div>
                   </div>
                  	<table id="inputparams" class="cb_dev_table" width="100%" >
           				<thead>
            				<tr><th>&nbsp;<th>Name</th><th>Type</th><th>Format</th><th>Missing</th><th>Padding</th><th>Required</th><th>Pos</th></tr>
           				</thead>
           				<tbody>
           				</tbody>
          			</table>
          			<table id="inputparams-edit" class="cb_dev_table" width="100%" >
           				<thead>
            				<tr><th>&nbsp;<th>Name</th><th>Type</th><th>Format</th><th>Missing</th><th>Padding</th><th>Required</th><th>Pos</th></tr>
           				</thead>
           				<tbody>
           				</tbody>
          			</table>
                </div>
            </div>
            <div id="tabs-General-row-9b">
              <div id="switchargs-panel">
                  <div id="switchargs-panel-header">
                    <div id="switchargs_title">
                    	<h2>Command Line Switches</h2>
                    </div>
                    <div id="switchargs_header_buttons">
                        	<button class="add_button" onClick="javascript:AddSwitchArgsRow('switchargs')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditSwitchArgsRow('switchargs')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteSwitchArgsRow('switchargs')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveSwitchArgsRow('switchargs')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelSwitchArgsRow('switchargs')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                     </div>
                   </div>
                   <table id="switchargs" class="cb_dev_table" width="100%" >
           				<thead>
            				<tr><th>&nbsp;</th><th>Name</th></tr>
           				</thead>
           				<tbody>
           				</tbody>
          			</table>
          			<table id="switchargs-edit" class="cb_dev_table" width="100%" >
           				<thead>
            				<tr><th>&nbsp;</th><th>Name</th></tr>
           				</thead>
           				<tbody>
           				</tbody>
          			</table>
                </div>
            </div>
            <div id="tabs-General-row-9c">
              <div id="cmdline-panel">
                 <div id="dragtextdiv" style="margin:10px">Drag the arguments from here onto the command line below to assemble the command. Use the table above to add and configure the switches/arguments.</div>
                 <div id="argsoutputpane"></div>
              </div>
              <div id="procbody-panel">
                 <div id="ProcBodyMainFrame" style="height:100%;width:100%;">
                    <textarea id='editproc' cols='400' wrap='off' spellcheck='false' style='width:100%;height:600px;font-family:courier;white-space: pre; overflow: auto;'></textarea>
                 </div>
              </div>
            </div>
            <div id="devops-details">Component with DevOps Details</div>
              <div id="tabs-General-row-10">
            	 <div id="domnav-panel">
     			</div>
                <div id="tabs-General-left">
                    <div id="tabs-General-left-header">
                    </div>
                    <div id="summ_data">
                        <table id="summ" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="summ_data_edit" style="display:none";>
                        <table id="summtab" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="compowner_summ_data">
                          <table id="compowner_summ" class="dev_table">
                            <tbody></tbody>
                          </table>
                     </div>
                     <div id="compowner_summ_data_edit" style="display:none";>
                          <table id="compowner_summtab" class="dev_table">
                            <tbody></tbody>
                          </table>
                    </div>
                    <div id="compitem_data">
	                    <table id="compitem" class="dev_table">
	                       <tbody></tbody>
	                    </table>
               		</div>
	                <div id="compitem_data_edit" style="display:none";>
	                   <table id="compitemtab" class="dev_table">
	                      <tbody></tbody>
	                   </table>
	               	</div>
                    <div id="emailbody_data">
                        <table id="emailbody" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="emailbody_data_edit" style="display:none";>
                        <table id="emailbodytab" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="props_data">
                        <table id="props" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="props_data_edit" style="display:none";>
                        <table id="propstab" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="adminrights_data">
                        <table id="adminrights" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="adminrights_data_edit" style="display:none";>
                        <table id="adminrightstab" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="servstat_data">
                        <table id="servstat" class="dev_table">
                            <thead><tr><th>Test Connection Results</th></tr></thead>
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="row-10b-access">
                      <div id="row-10-baccess_title" style="text-align:left"">
                            <h2>Access Control</h2>
                      </div>
                      <div id="row-10b-access-data">
                      </div>
                   </div>
                </div>
              </div>
              <div id="row-10-right-panel">
                <div id="emailto-panel">
                  <div id="emailto-panel-header">
                    <div id="emailto_title" style="text-align:left"">
                            <h2>Notifier Recipients</h2>
                    </div>
                    <div id="emailto_header_buttons">
                        	<button class="add_button" onClick="javascript:AddEmailToRow('emailto')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditEmailToRow('emailto')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteEmailToRow('emailto')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveEmailToRow('emailto')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelEmailToRow('emailto')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    </div>
                  </div>
                    <div>
                        <table id="emailto" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Recipients</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="emailto-edit" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Recipients</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
                <div id="groupmembership-panel">
                  <div id="groupmembership-panel-header">
                    <div id="groupmembership_title" style="text-align:left"">
                            <h2>Group Membership</h2>
                    </div>
                    <div id="groupmembership_header_buttons">
                        	<button class="add_button" onClick="javascript:AddGroupMembershipRow('groupmembership')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditGroupMembershipRow('groupmembership')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteGroupMembershipRow('groupmembership')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveGroupMembershipRow('groupmembership')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelGroupMembershipRow('groupmembership')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    </div>
                  </div>
                    <div>
                        <table id="groupmembership" class="dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Groups for User</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="groupmembership-edit" class="dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Groups for User</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
                <div id="usermembership-panel">
                  <div id="usermembership-panel-header">
                    <div id="usermembership_title" style="text-align:left">
                            <h2>User Membership</h2>
                    </div>
                    <div id="usermembership_header_buttons">
                        	<button class="add_button" onClick="javascript:AddUserMembershipRow('usermembership')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditUserMembershipRow('usermembership')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteUserMembershipRow('usermembership')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveUserMembershipRow('usermembership')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelUserMembershipRow('usermembership')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    </div>
                  </div>
                    <div>
                        <table id="usermembership" class="dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Users for Group</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="usermembership-edit" class="dev_table">
                            <thead>
                                <tr>
                                <th>&nbsp;</th>
                                    <th>Users for Group</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
                 <div id="attrs10-panel">
                 	<div id="attrs10-header">
                     	<div id="attrs10_title">
                      	  <h2>Key Value Configuration</h2>
                    	</div>
                    	<div id="attrs10_header_buttons">
                        	<button class="add_button" onClick="javascript:AddAttrRow('attrib10')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditAttrRow('attrib10')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteAttrRow('attrib10')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveAttrRow('attrib10')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelAttrRow('attrib10')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    	</div>
                    </div>
                    <div id="attrib10-data">
                        <table id="attrib10" class="dev_table" width="100%" >
                            <thead>
                                <tr>
                                    <th>Name</th>
                                    <th>Value</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
            </div>

            <div id="tabs-General-row-50">
		       <div id="panel_container_taskspaneltop">
				<div id="tasks-panel">
				  <div id="tasks-header">
                     	<div id="tasks_title">
                      	  <h2>Tasks</h2>
                    	</div>
                    	<div id="tasks_header_buttons">
                        	<div class="dropdown_menu">
                              <button class="title_buttons addMenuButton" onclick="addTaskMenu()" style="color:rgb(51, 103, 214)"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
                                <div class="dropdown_menu-content" style="display: none;">
                                  <a onclick="AddTaskRow('taskstab','Deploy');">Deploy Version to an Environment</a>
                                  <a onclick="AddTaskRow('taskstab','Request');">Request Calendar entry for deployment to an Environment</a>
                                  <a onclick="AddTaskRow('taskstab','RunAction');">Mannually trigger Action to be executed</a>
                               </div>
                            </div>
                        	<button class="delete_button" onClick="javascript:DeleteTaskRow('taskstab')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                    	</div>
                    </div>
                    <div id="taskstab-data">
                        <table id="taskstab" class="cb_dev_table" width="100%" >
                            <thead>
                                <tr>
                                <th>&nbsp;</th>
                                    <th>Name</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
				</div>
				<div id="panel_deploy">
				     <div id="task-right-header">
                        <div id="task_summ_title">
                            <h2>Task Details</h2>
                        </div>
                        <div id="task_summ_header_buttons">
                            <button class="edit_button" onClick="javascript:EditSummaryButton(false,'task_')">
                                <i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit
                            </button>
                            <button class="save_button" onClick="javascript:summOK(false,'task_')">
                                <i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save
                            </button>
                            <button class="cancel_button" onClick="javascript:summCancel('task_')">
                                <i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel
                            </button>
                        </div>
                    </div>
                    <div id="task_summ_data">
                        <table id="task_summ" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="task_summ_data_edit" style="display:none";>
                        <table id="task_summtab" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
				</div>
			</div>
		</div>
		<div id="tabs-General-row-51">
			<div id="panel_container_taskspanelbottom">
				<div id="sub_panel_bottomleft">
				  <div class="panel_container_title_area tasks"><h4 class="taskpanels">Group Access for Task</h4></div>
				   <div class="deploy_sub_panel_container">
				    <ul class="subpanel" id="tasks_assigned_groups">
	 			    </ul>
	 			   </div>
				</div>
				<div id="sub_panel_bottommiddle">
				 	<div class="panel_container_title_area"><h4 class="taskpanels">Available Groups for Task</h4></div>
				 	<div class="deploy_sub_panel_container">
				 		<ul class="subpanel" id="tasks_available_groups">
	 					</ul>
	 				</div>
	 			</div>
	 			<div id="sub_panel_bottomright">
	 			    <div class="panel_container_title_area"><h4 class="taskpanels">Task Parameters</h4></div>
				 	<div id="task_params"></div>
				 	<div id="task_param_content"></div>
	 			</div>
			 </div>
		</div>

           <div id="tabs-General-row-14">
              <div id="logs-panel">
                 <div id="logs_title" style="text-align:left"">
                     <h2>Log History</h2>
                 </div>
                 <div id="logs-data">
                 <table id="loghist" class="dev_table" width="100%">
                     <thead>
                         <tr>
                             <th>Deployment</th>
                             <th>Result</th>
                             <th>Date</th>
                             <th>Environment</th>
                         </tr>
                     </thead>
                     <tbody></tbody>
                 </table>
                 </div>
              </div>
           </div>
           <div id="tabs-General-row-15">
              <div id="attrs-panel">
                 	<div id="attrs-header">
                     	<div id="attrs_title">
                      	  <h2>Key Value Configuration</h2>
                    	</div>
                    	<div id="attrs_header_buttons">
                        	<button class="add_button" onClick="javascript:AddAttrRow('attrib')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditAttrRow('attrib')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteAttrRow('attrib')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveAttrRow('attrib')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelAttrRow('attrib')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    	</div>
                    </div>
                    <div id="attrib-data">
                        <table id="attrib" class="cb_dev_table" width="100%" >
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Name</th>
                                    <th>Value</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="attrib-edit" class="cb_dev_table" width="100%">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Name</th>
                                    <th>Value</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
            </div>
      <div id="audit-access">Audit and Access</div>
           <div id="tabs-General-row-35">
                <div id="row-35-audit">
                        <div id="audit35_title" style="text-align:left"">
                            <h2>Audit Trail</h2>
                        </div>
                        <div id="audit35-data">
                          <div id="audit35-historyadd" style="width:96%;height:100px;display:inline""></div>
                          <table id="audit35-histfeed" class="dev_table" style="display:none"">
                               <tbody></tbody>
                          </table>
                        <table id="audit35" class="dev_table" width="100%">
                            <thead>
                                <tr>
                                    <th>Event</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        </div>
                </div>
          </div>
               <div id="tabs-General-row-35a">
                <div id="row-35-access">
                    <div id="row-35-access_title" style="text-align:left"">
                            <h2>Access Control</h2>
                    </div>
                    <div id="row-35-access-data">
                    </div>
                </div>
               </div>
          </div>
                    </div>
                </div>

       <div id="tabs-PackageComponents-data" style="display:none">
        <div class="panel_container versionsandinformation">
     	 <div class="compversions">
	     <div id="innercomp">
		 </div>
	    </div>
	    <div class="accordionpanel">
	     <h4 class="ui-widget-header">Component Selector</h4>
   	     <div id=componentlist align="left" style="margin:0; overflow-x: auto; height: calc(100% - 52px); background-color: #f8f8f8;"></div>
        </div>
       </div>
      </div>
		 </div>
		</div>
       </div>
     </div>
	</div> <!-- end panel container -->
	<div id="panel_container_historyandpending" style="display:none">
	 <div id="home_right_panel_tabs">
	  <div class="tabrow_tabmenu">
	   <div class="moduletable_tabmenu">
	    <ul>
	      <li id="tabs-HomeDashboard" onclick="ActivateHomeSubTabs('tabs-HomeDashboard')" class="active current">Global Reports</li>
          <li id="tabs-HomePending" onclick="ActivateHomeSubTabs('tabs-HomePending')">To Do List</li>
          <li id="tabs-HomeHistory" onclick="ActivateHomeSubTabs('tabs-HomeHistory')">Audit Trail</li>
        </ul>
	   </div>
	  </div>
	 </div>
   </div>
   </div>  <!-- end container -->
	<div class="clear">
	</div>
	<div id="login_container_dialog"></div>
	<div id="firstinstall_container_dialog"></div>
	<div id="modal" style="display:none"></div>
	<div id="rel-upgrade" title="Upgrade to Pro" class="dmdialog" style="display:none">
 		 <h2 style="padding-right:30px">Please upgrade to the Pro version in order to create Releases.</h2>
	     <form>
	     </form>
	</div>
	<div id="dom-upgrade" title="Upgrade to Pro" class="dmdialog" style="display:none">
 		 <h2 style="padding-right:30px">Please upgrade to the Pro version in order to create project subfolders.</h2>
	     <form>
	     </form>
	</div>
	<div id="displaylog"></div>
	<div id="buildlog"></div>
	<div id="throbber" style="display:none;">
    	<img src="css/images/dog.gif" style="height:80px;border-radius: 25px;" />
    	<p style="position:relative;left:-7px;">Sit tight....</p>
	</div>
</body>
</html>
